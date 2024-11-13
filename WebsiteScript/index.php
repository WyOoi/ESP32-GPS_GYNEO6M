<?php
require 'config.php';

$sql = "SELECT * FROM tbl_gps WHERE 1";
$result = $db->query($sql);
if (!$result) {
    echo "Error: " . $sql . "<br>" . $db->error;
}

$row = $result->fetch_assoc();
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>UTeM Bus Tracker</title>
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Roboto', sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f4f4f4;
            color: #333;
        }

        header {
            background-color: #2c3e50;
            color: white;
            text-align: center;
            padding: 20px;
            font-size: 24px;
        }

        #map-layer {
            width: 100%;
            height: 500px;
            border-radius: 8px;
            box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);
            margin: 20px auto;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }

        .loading {
            display: none;
            text-align: center;
            font-size: 18px;
            color: #3498db;
        }

        /* Centered Bus Information Title */
        .bus-info-title-container {
            background-color: #3498db; /* Different background color */
            color: #fff;
            padding: 10px;
            text-align: center;
            font-size: 18px;
            font-weight: bold;
            border-radius: 8px;
            margin-bottom: 10px;
            max-width: 300px;
            margin-left: auto;
            margin-right: auto;
        }

        /* Original Bus Plate and Route Information */
        .bus-info-container {
    		display: flex;
    		justify-content: center; /* Centers the boxes horizontally */
    		gap: 10px; /* Adds space between boxes */
    		flex-wrap: wrap; /* Allows wrapping on smaller screens */
		}

		.bus-info {
		    width: 250px; /* Adjust width to fit four boxes in a row if needed */
		    background-color: #fff;
		    padding: 20px;
		    border-radius: 8px;
		    box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);
		    text-align: left;
		}

        .bus-info-details {
            font-size: 16px;
            line-height: 1.6;
        }

        footer {
            background-color: #2c3e50;
            color: white;
            text-align: center;
            padding: 15px;
            position: relative;
            bottom: 0;
            width: 100%;
        }

        @media (max-width: 768px) {
            #map-layer {
                height: 300px;
            }
        }
    </style>
</head>
<body>

<header>
    UTeM Bus Tracker
</header>

<div class="container">
    <div class="loading" id="loading">Loading map...</div>
    <div id="map-layer"></div>
    
    <!-- Centered Bus Information Title Container -->
    <div class="bus-info-title-container">
        Bus Information
    </div>

    <!-- Flex container for bus info boxes -->
    <div class="bus-info-container">
        <div class="bus-info">
            <div class="bus-info-details">
                <p><strong>Bus Plate:</strong> UTEM 1234</p>
                <p><strong>Bus Route:</strong> Satria to Kampus Induk</p>
                <p><strong>Status:</strong> Available</p>
            </div>
        </div>

        <div class="bus-info">
            <div class="bus-info-details">
                <p><strong>Bus Plate:</strong> UTEM 1233</p>
                <p><strong>Bus Route:</strong> Kampus Induk to Satria</p>
                <p><strong>Status:</strong> Unavailable</p>
            </div>
        </div>

        <div class="bus-info">
            <div class="bus-info-details">
                <p><strong>Bus Plate:</strong> UTEM 1244</p>
                <p><strong>Bus Route:</strong> Al-Jazari to Kampus Induk</p>
                <p><strong>Status:</strong> Unavailable</p>
            </div>
        </div>

        <div class="bus-info">
            <div class="bus-info-details">
                <p><strong>Bus Plate:</strong> UTEM 1222</p>
                <p><strong>Bus Route:</strong> Kampus Induk to Al-Jazari</p>
                <p><strong>Status:</strong> Unavailable</p>
            </div>
        </div>
    </div>
</div>

<footer>
    &copy; 2024 UTeM Bus Tracker | All Rights Reserved
</footer>

<script src="https://maps.googleapis.com/maps/api/js?key=<?php echo GOOGLE_MAP_API_KEY; ?>&callback=initMap" async defer></script>

<script>
    var map;
    var markers = [];

    function initMap() {
        var mapLayer = document.getElementById("map-layer");
        var centerCoordinates = new google.maps.LatLng(<?php echo $row['lat']; ?>, <?php echo $row['lng']; ?>);
        var defaultOptions = { 
            center: centerCoordinates, 
            zoom: 10 
        };

        map = new google.maps.Map(mapLayer, defaultOptions);
        
        // Hide the loading message after the map is loaded
        document.getElementById('loading').style.display = 'none';

        // Loop through the rows to place markers
        <?php 
        // Move the pointer to the beginning of the result again
        $result->data_seek(0); 
        while($row = $result->fetch_assoc()) { 
        ?>
            var location = new google.maps.LatLng(<?php echo $row['lat']; ?>, <?php echo $row['lng']; ?>);
            var marker = new google.maps.Marker({
                position: location,
                map: map,
                title: 'Bus Location'
            });

            // Add info window to each marker
            var infowindow = new google.maps.InfoWindow({
                content: '<strong>Bus Location</strong><br>Latitude: <?php echo $row['lat']; ?><br>Longitude: <?php echo $row['lng']; ?>'
            });

            marker.addListener('click', function() {
                infowindow.open(map, marker);
            });

            markers.push(marker);
        <?php } ?>
    }
</script>

</body>
</html>
